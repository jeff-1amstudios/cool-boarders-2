"""Bridge a single Codex app-server turn to the decomp worker's JSONL log."""
import json
import subprocess
import sys
from pathlib import Path


def emit(kind, **fields):
    print(json.dumps(dict(type=kind, **fields)), flush=True)


def run(budget, server_command):
    prompt = sys.stdin.read()
    # The supervisor owns this process group, including the server and tools.
    server = subprocess.Popen(server_command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                              text=True, encoding="utf-8")

    def send(**message):
        print(json.dumps(message), file=server.stdin, flush=True)

    send(id=1, method="initialize", params={
        "clientInfo": {"name": "cb2_decomp", "version": "1"}})
    thread = None
    usage = None
    for line in server.stdout:
        message = json.loads(line)
        method, request_id = message.get("method"), message.get("id")
        if method and request_id is not None:
            send(id=request_id, error={"code": -32601, "message": "Interactive requests are unavailable"})
            continue
        if "error" in message:
            raise RuntimeError(message["error"]["message"])
        if request_id == 1:
            send(method="initialized")
            send(id=2, method="thread/start", params={
                "cwd": str(Path.cwd()), "approvalPolicy": "never",
                "sandbox": "workspace-write", "ephemeral": True})
        if request_id == 2:
            thread = message["result"]["thread"]["id"]
            send(id=3, method="turn/start", params={
                "threadId": thread, "input": [{"type": "text", "text": prompt, "text_elements": []}]})
        params = message.get("params", {})
        if not thread or params.get("threadId") != thread:
            continue
        if method == "thread/tokenUsage/updated":
            total = params["tokenUsage"]["total"]
            usage = dict(input_tokens=total["inputTokens"], cached_input_tokens=total["cachedInputTokens"],
                         output_tokens=total["outputTokens"], reasoning_output_tokens=total["reasoningOutputTokens"])
            emit("usage.updated", usage=usage)
            spent = usage["input_tokens"] - usage["cached_input_tokens"] + usage["output_tokens"]
            if spent >= budget:
                # Exit immediately. The supervisor kills our process group before
                # inspecting candidate edits, so no tool can continue writing.
                emit("budget.reached", budget=budget, spent=spent)
                return
        elif method in ("item/started", "item/completed"):
            item = params["item"]
            kind = method.replace("/", ".")
            if item["type"] == "agentMessage" and method == "item/completed":
                emit(kind, item=dict(type="agent_message", text=item["text"]))
            elif item["type"] == "commandExecution":
                emit(kind, item=dict(type="command_execution", command=item["command"],
                                     aggregated_output=item["aggregatedOutput"] or "",
                                     exit_code=item["exitCode"]))
        elif method == "error" and not params.get("willRetry"):
            emit("error", message=params["error"]["message"])
        elif method == "turn/completed":
            turn = params["turn"]
            if turn["status"] != "completed":
                raise RuntimeError((turn.get("error") or {}).get("message") or f"Turn {turn['status']}")
            if usage is None:
                raise RuntimeError("Codex completed the turn without a token usage update")
            emit("turn.completed", usage=usage)
            return
    raise RuntimeError("Codex app-server closed before completing the turn")


if __name__ == "__main__":
    try:
        run(int(sys.argv[1]), sys.argv[2:])
    except (OSError, RuntimeError, ValueError, KeyError) as error:
        emit("turn.failed", error={"message": str(error)})
        sys.exit(1)
