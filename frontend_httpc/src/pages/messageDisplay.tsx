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

interface messageObject {
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
  const [messagesObject, setMessagesObject] = useState<messageObject[]>([]);

  const [connectedUsersList, setConnectedUsersList] = useState<ConnectedUsers[]>([]);

  // Tabs
  const [activeTab, setActiveTab] = useState<string | null>(null);
  const [tabMessages, setTabMessages] = useState<messageObject[]>([]);

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
    registerUserReq(username);

    setUserName(username || "");
  }, [userName]);

  useEffect(() => {
    getUserListReq(userName);

  }, [userName])


  return (
    <section className="hud-layout">
      <aside className="hud-sidebar">
        <NetworkStatus connectionStatus={connectionStatus} />
        <UserSelection userName={userName} setUserName={setUserName} userList={connectedUsersList} />
      </aside>

      <div className="hud-main-panel">
        <MessageFeed 
          socket={socket} connectionStatus={connectionStatus}
          messagesObject={messagesObject} setMessagesObject={setMessagesObject}
          connectedUsersList={connectedUsersList} activeTab={activeTab} setActiveTab={setActiveTab}
          tabMessages={tabMessages} setTabMessages={setTabMessages}
        />
      </div>

      <footer className="hud-footer">
        <MessageBox socket={socket} connectionStatus={connectionStatus} userName={userName}
        setMessagesObject={setMessagesObject} activeTab={activeTab}/>
      </footer>
    </section>
  );
}

export default MessageDisplay;
