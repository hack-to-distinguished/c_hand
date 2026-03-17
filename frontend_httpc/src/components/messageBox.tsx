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
  const [showQuoteWarning, setShowQuoteWarning] = useState<boolean>(false);
  
  const flashQuoteWarning = () => {
    setShowQuoteWarning(true);
    window.setTimeout(() => setShowQuoteWarning(false), 2000);
  };

  const handleSendMessage = async (e: React.FormEvent) => {
    try {
      e.preventDefault();
      const trimmed = currentMessage.trim();
      if (!trimmed) return;

      const messageObj = {
        sender_id: userName || "unknown",
        send_time: new Date().toISOString(),
        // include client-side timestamp (ISO) — server can overwrite/augment if needed
        user_message: trimmed,
      };

      await sendMessage({ socket, message: messageObj });
      setCurrentMessage("");
    } catch (err) {
      console.log(`Error in sendMessage function: ${err}`);
    }
  };
  
  // Blocking chars
  const onKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
     if (e.key === '"') {
       e.preventDefault();
       flashQuoteWarning();
     }
   };
  const onPaste = (e: React.ClipboardEvent<HTMLInputElement>) => {
    const paste = e.clipboardData.getData("text");
     if (paste.includes('"')) {
       e.preventDefault();
       flashQuoteWarning();
     }
   };
  const onChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const val = e.target.value;
    if (val.includes('"')) {
      flashQuoteWarning();
      return;
    }
    setCurrentMessage(val);
  }

  return (
    <div className="send-msg-container">
      <form onSubmit={handleSendMessage} className="message-form">
        <div className="form-heading">Message Input</div>
        <input
          className="msg-input-box"
          type="text"
          value={currentMessage}
          placeholder="Type a message..."
          onChange={onChange}
          onKeyDown={onKeyDown}
          onPaste={onPaste}
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
        >
          {showQuoteWarning ? 'Double-quote (") characters are not allowed.' : ""}
        </div>
      </form>
    </div>
  );
};

export default MessageBox;