import { useState, useEffect } from "react";
// Components
import MessageBox from "../components/messageBox.tsx";
import MessageFeed from "../components/messageFeed.tsx";
import UserSelection from "../components/userSelection.tsx";
// Services
import { useWebSocket } from "../services/handleNetwork.tsx";
import NetworkStatus from "../components/networkStatus.tsx";
import { setInLocalStorage } from "../services/handleBrowser.tsx";
import { generateId, getAllUsers, registerUser } from "../services/handleUser.tsx";

import "./messageDisplay.css";

interface SavedMessages {
  send_time?: string;
  sender_id?: string;
  message?: string;
  [key: string]: any;
}

interface ConnectedUsers {
  username: string;
  lastActiveTime: string;
}

function MessageDisplay() {
  const serverUrl = "ws://localhost:8081";
  const { socket, connectionStatus } = useWebSocket(serverUrl);

  const [userName, setUserName] = useState<string>("");
  const [messagesObject, setMessagesObject] = useState<SavedMessages[]>([]);

  const [connectedUsersList, setConnectedUsersList] = useState<ConnectedUsers[]>([]);

  const registerUserReq = async (username: string) => {
    try {
      await registerUser(username);
    } catch (error) {
      console.log("Registering user error", error);
    }
  }

  const getUserListReq = async (username: string) => {
    try {
      const userList = await getAllUsers(username);
      setConnectedUsersList(userList);
    } catch (error) {
      console.log("Getting list of users error", error);
    }
  }

  useEffect(() => {
    // initialize username from localStorage (or generate one)
    if (userName) return;

    let username = localStorage.getItem("username");
    if (!username) {
      username = generateId(5);
      setInLocalStorage("username", username);
    }
    // TODO: make initial register post request in this use effect
    registerUserReq(username);

    setUserName(username || "");
  }, [userName]);

  useEffect(() => {
    getUserListReq(userName);

  }, [userName])


  return (
    <>
      <UserSelection userName={userName} setUserName={setUserName} userList={connectedUsersList} />
      <NetworkStatus connectionStatus={connectionStatus} />
      <MessageFeed socket={socket} connectionStatus={connectionStatus} messagesObject={messagesObject} setMessagesObject={setMessagesObject} />
      <MessageBox socket={socket} connectionStatus={connectionStatus} userName={userName} setMessagesObject={setMessagesObject} />
    </>
  );
}

export default MessageDisplay;
