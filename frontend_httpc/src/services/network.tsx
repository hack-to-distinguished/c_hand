import { useEffect, useState, useRef } from "react";

export function useWebSocket(serverUrl: string) {
  const [connectionStatus, setConnectionStatus] = useState("Disconnected");
  const socket = useRef<WebSocket | null>(null);
  // const reconnectTimeout = useRef<NodeJS.Timeout | null>(null);
  // const shouldReconnect = useRef(true); // Track if we should reconnect

  useEffect(() => {
    const ws = new WebSocket(serverUrl);
    socket.current = ws;

    setConnectionStatus("Connecting...");

    ws.onopen = () => {
      if (socket.current === ws) {
        console.log("WebSocket connection established!");
        setConnectionStatus("Connected");
      }
    };

    ws.onclose = (event) =>{
      if (socket.current === ws) {
        console.log("WeSocket connection closed", event.code, event.reason);
        setConnectionStatus("Disconnected");
      }
    };

    ws.onerror = (error) => {
      if (socket.current === ws) {
        console.error("WebSocket error: ", error);
        setConnectionStatus("Error");
      }
    };

    return () => {
      if ( ws.readyState === WebSocket.OPEN ||  ws.readyState === WebSocket.CONNECTING) {
        ws.close(1000, "Component unmounting");
      }

      if (socket.current === ws) {
        socket.current = null;
      }
    };

  }, [serverUrl]);

  return { socket, connectionStatus };
}
