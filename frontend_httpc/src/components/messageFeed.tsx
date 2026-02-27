import { useState, useEffect} from "react";
import { getAllMessages } from "../services/getMessages.tsx";
import "./messageFeed.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
}

interface SavedMessages {
  message?: string;
  send_time?: string;
  sender_id?: string;
}

const MessageFeed = ({ socket }: MessageBoxProps) => {
  const [messages, setMessages] = useState<string[]>([]);
  const [messagesObject, setMessagesObject] = useState<SavedMessages[]>([]);
  const [completedInitialRequest, setCompletedInitialRequest] = useState<boolean>(false);

  const initialGetMessagesReq = async () => {
    try {
      console.log("Attempting to get all messages")
      const messages = await getAllMessages();
      if (messages) {
        console.log("All messages:", messages);

        const userMessages = messages.map((userObj) => userObj.message);

        const messageCount = Object.keys(messages).length;
        console.log(`There are ${messageCount} messages`);
        const mapped = messages.map((messages: SavedMessages) => messages);
        setMessagesObject(mapped);

        setMessages(userMessages)
        setCompletedInitialRequest(true);
      } 

    } catch (error) {
      console.log("Unable to get messages:", error);
    }
  };

  useEffect(() => {
    if (completedInitialRequest) return;
    console.log("request not done", completedInitialRequest);
    initialGetMessagesReq();
  }, [completedInitialRequest]);

  useEffect(() => { // Only used for logging whenever messageObject changes
    console.log("message object", messagesObject);
  }, [messagesObject]);

  useEffect(() => {
    if (!socket.current) return;

    const handleMessage = (event: MessageEvent) => {
      const receivedMessage = event.data;
      console.log("Event", socket.current);
      console.log("Message from server:", receivedMessage);
      const now = new Date();
      const send_time = `${now.toLocaleString("en-US", {
        month: "short",
        day: "2-digit"
      })} ${now
        .toTimeString()
        .split(" ")[0]
        .replace(/:/g, "-")} ${now.getFullYear()}`;

      setMessagesObject(messagesObject => [
        ...messagesObject, // (spread operator) needed to make sure we don't overwrite all the values our object
        {message: receivedMessage, send_time: send_time, sender_id: "you"}
      ]);
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
        {Object.keys(messagesObject).length === 0 ? (
          <li className="empty-message">No messages yet</li>
          ) : (
            messagesObject.map((message, i) => (
            <li key={i}>
              <span style= {{ color: "#003366", fontWeight: "bold" }}>
                {message.send_time} - From { message.sender_id }: 
              </span>{" "}
                {message.message}
            </li>
            ))
          )
        }
      </ul>
    </div>
  );
};

export default MessageFeed;
