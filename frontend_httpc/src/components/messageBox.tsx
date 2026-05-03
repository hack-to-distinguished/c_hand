import React, { useState } from "react";
import { sendMessage } from "../services/handleMessages.tsx";
import "./messageBox.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
  userName: string;
  send_time?: string;
  sender_id?: string;
  message?: string;
  [key: string]: any;
}

const MessageBox = ({ socket, connectionStatus, userName, setMessagesObject }: MessageBoxProps) => {
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
        recipient_id: "all",
        message: trimmed,
      };

      // TODO: Add the destination client_fd and their recipient_id to the payload
      // the client fd will be used to decided who to send the message to in wsock_server.c
      // and the recipient_id will be used by the initial get request when switching tabs
      // if no client_fd specified or client_fd = -1, the message is directed at everyone
      // if no recipient_id is specified or recipient_id = Null, the message is directed at everyone

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
