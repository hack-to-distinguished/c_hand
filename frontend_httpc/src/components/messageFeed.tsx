import { useState, useEffect} from "react";
import { getAllMessages } from "../services/getMessages.tsx";
import "./messageFeed.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
}

const MessageFeed = ({ socket }: MessageBoxProps) => {
  const [messages, setMessages] = useState<string[]>([]);
  const [completedInitialRequest, setCompletedInitialRequest] = useState<boolean>(false);

  const initialGetMessagesReq = async () => {
    try {
      console.log("Attempting to get all messages")
      const messages = await getAllMessages();
      if (messages) {
        console.log("All messages:", messages);

        const userMessage = messages.reverse().map((filObj) =>filObj.message);
        setMessages(userMessage)
      } 

    } catch (error) {
      console.log("Unable to get messages:", error);
    }
  };

  useEffect(() => {
    if (completedInitialRequest) return;
    console.log("request not done", completedInitialRequest);
    initialGetMessagesReq();
    setCompletedInitialRequest(true);
  }, [completedInitialRequest]);

  useEffect(() => {
    if (!socket.current) return;

    const handleMessage = (event: MessageEvent) => {
      const receivedMessage = event.data;
      console.log("Event", socket.current);
      console.log("Message from server:", receivedMessage);
      setMessages((prevMessages) => [...prevMessages, receivedMessage]);
    };

    socket.current.onmessage = handleMessage;

    return () => {
      if (socket.current) {
        socket.current.onmessage = null;
      }
    };
  }, [socket.current]);

  return (
    <div className="messages-display">
      <div className="message-header">HTTP_C Chat</div>
      <ul className="messages-list">
        {messages.length === 0 ? (
          <li className="empty-message">No messages yet.</li>
        ) : (
          messages.map((msg, i) => (
            <li key={i}>
              <span style={{ color: "#003366", fontWeight: "bold" }}>
                {new Date().toLocaleTimeString()} - Message {i + 1}:
              </span>{" "}
              {msg}
            </li>
          ))
        )}
      </ul>
    </div>
  );
};

export default MessageFeed;
