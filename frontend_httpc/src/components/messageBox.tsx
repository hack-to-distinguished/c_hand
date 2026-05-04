import React, { useState } from "react";
import { sendMessage } from "../services/handleMessages.tsx";
import "./messageBox.css";

interface MessageObject {
  send_time?: string;
  sender_id?: string;
  message?: string;
  [key: string]: any;
}

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
  userName: string;
  setMessagesObject: React.Dispatch<React.SetStateAction<MessageObject[]>>;
  activeTab: string | null;
}

const MessageBox = ({ socket, connectionStatus, userName, setMessageObject, activeTab }: MessageBoxProps) => {
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

      const now = new Date();
      const month = now.toLocaleString('en-UK', { month: 'short' });
      const day = now.getDate().toString().padStart(2, '0');
      const year = now.getFullYear();
      const time = now.toTimeString().split(' ')[0];

      const formattedNow = `${month} ${day} ${time} ${year}`;

      const messageObj = {
        sender_id: userName || "unknown",
        send_time: formattedNow,
        recipient_id: activeTab,
        message: trimmed,
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
        <div className="quote-warning">
          {showQuoteWarning ? 'Double-quote (") characters are not allowed.' : ""}
        </div>
      </form>
    </div>
  );
};

export default MessageBox;
