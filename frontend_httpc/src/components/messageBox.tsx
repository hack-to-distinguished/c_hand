import React, { useState } from "react";
import { sendMessage } from "../services/recv_send.tsx";
import "./messageBox.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
  userName: string;
}

const MessageBox = ({ socket, connectionStatus, userName }: MessageBoxProps) => {
  const [currentMessage, setCurrentMessage] = useState<string>("");

  const handleSendMessage = async (e: React.FormEvent) => {
    try {
      e.preventDefault();
      const trimmed = currentMessage.trim();
      if (!trimmed) return;

      // Structured message object
      const messageObj = {
        sender_id: userName || "unknown",
        message: trimmed,
        // include client-side timestamp (ISO) — server can overwrite/augment if needed
        send_time: new Date().toISOString(),
      };

      await sendMessage({ socket, message: messageObj });
      setCurrentMessage("");
    } catch (err) {
      console.log(`Error in sendMessage function: ${err}`);
    }
  };

  return (
    <div className="send-msg-container">
      <form onSubmit={handleSendMessage} className="message-form">
        <div className="form-heading">Message Input</div>
        <input
          className="msg-input-box"
          type="text"
          value={currentMessage}
          placeholder="Type a message..."
          onChange={(e) => setCurrentMessage(e.target.value)}
        />
        <button
          type="submit"
          disabled={connectionStatus !== "Connected"}
          className="send-msg-button"
        >
          Send Message
        </button>
        <div
          style={{
            textAlign: "left",
            marginTop: "10px",
            fontSize: "10px",
            color: "#666",
          }}
        ></div>
      </form>
    </div>
  );
};

export default MessageBox;