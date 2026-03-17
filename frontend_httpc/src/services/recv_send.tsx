type SocketRef = React.RefObject<WebSocket | null>;
type MessageObject = {
  sender_id?: string;
  send_time?: string; // ISO string preferred
  user_message?: string;
  [key: string]: any;
};

export const handleMessage = ({
  socket,
  setMessages,
  event,
}: {
  socket: SocketRef;
  setMessages: React.Dispatch<React.SetStateAction<MessageObject[]>>;
  event: MessageEvent;
}) => {
  if (!socket?.current) return;

  let parsed: MessageObject | null = null;
  try {
    parsed = JSON.parse(event.data);
  } catch (err) {
    // Fallback: treat the incoming data as a raw message string
    parsed = { message: event.data };
  }

  // Ensure we append a consistent object shape
  const now = new Date();
  const month = now.toLocaleString('en-UK', { month: 'short' });
  const day = now.getDate().toString().padStart(2, '0');
  const year = now.getFullYear();
  const time = now.toTimeString().split(' ')[0];

  const formattedNow = `${month} ${day} ${time} ${year}`;

  const messageObj: MessageObject = {
    sender_id: parsed.sender_id ?? "unknown",
    send_time: parsed.send_time ?? formattedNow,
    user_message: parsed.message ?? "",
    ...parsed,
  };

  setMessages((prevMessages) => [...prevMessages, messageObj]);
};

// Send a structured message object over the websocket (JSON-stringified)
export const sendMessage = async ({
  socket,
  message,
}: {
  socket: SocketRef;
  message: MessageObject;
}) => {
  if (!socket?.current) {
    console.log("Cannot send message. No socket available.");
    return;
  }

  if (socket.current.readyState === 1) {
    try {
      const payload = JSON.stringify(message);
      socket.current.send(payload);
      console.log("Sent JSON message:", payload);
    } catch (err) {
      console.log("Error stringifying/sending message:", err);
    }
  } else {
    console.log("Cannot send message. WebSocket is not open.");
  }
};