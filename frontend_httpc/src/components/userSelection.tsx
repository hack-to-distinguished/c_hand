import React, { useState, useEffect, useRef } from "react";
import { setInLocalStorage } from "../services/handleBrowser";
import "./userSelection.css";
import { changeUsername } from "../services/handleUser";

interface ConnectedUser {
  username: string;
  lastActiveTime: string;
}

interface UserSelectionProps {
  userName: string;
  setUserName: (name: string) => void;
  userList: ConnectedUser[];
}

const UserSelection = ({ userName, setUserName, userList }: UserSelectionProps) => {
  const [isEditing, setIsEditing] = useState(false);
  const [editValue, setEditValue] = useState(userName);
  const [showUsers, setShowUsers] = useState(false);
  const inputRef = useRef<HTMLInputElement | null>(null);

  useEffect(() => {
    setEditValue(userName);
  }, [userName]);

  useEffect(() => {
    if (isEditing) {
      inputRef.current?.focus();
      // place cursor at end
      const len = inputRef.current?.value.length ?? 0;
      inputRef.current?.setSelectionRange(len, len);
    }
  }, [isEditing]);
  
  
  const changeUserReq = async (username: string, newUsername: string) => {
    try {
      console.log("Requesting username change from %s to %s", username, newUsername)
      await changeUsername(username, newUsername);
    } catch (error) {
      console.log("Registering user error", error);
    }
  }

  const save = () => {
    const trimmed = editValue.trim();
    if (trimmed && trimmed !== userName) {
      // TODO: Add the call to check here
      changeUserReq(userName, trimmed);
      setUserName(trimmed);
      setInLocalStorage("username", trimmed);
    }
    setIsEditing(false);
  };

  const cancel = () => {
    setEditValue(userName);
    setIsEditing(false);
  };

  const onInputKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === "Enter") {
      e.preventDefault();
      save();
    } else if (e.key === "Escape") {
      e.preventDefault();
      cancel();
    }
  };

  const onNameKeyDown = (e: React.KeyboardEvent<HTMLSpanElement>) => {
    if (e.key === "Enter" || e.key === " ") {
      e.preventDefault();
      setIsEditing(true);
    }
  };

  return (
    <div className="user-selection">
      <div className="user-selection-row">
        <strong>User:</strong>
        {isEditing ? (
          <input
            ref={inputRef}
            className="user-name-input"
            value={editValue}
            onChange={(e) => setEditValue(e.target.value)}
            onBlur={save}
            onKeyDown={onInputKeyDown}
            aria-label="Edit display name"
            title="Type a new name then press Enter to save or Escape to cancel"
          />
        ) : (
          <span
            className="user-name"
            role="button"
            tabIndex={0}
            onClick={() => setIsEditing(true)}
            onKeyDown={onNameKeyDown}
            aria-label="Change display name"
            title="Click or press Enter to change name"
          >
            {userName}
          </span>
        )}
      </div>
      
      <div className="connected-users">
        <button
          type="button"
          className="toggle-users"
          onClick={() => setShowUsers((s) => !s)}
          aria-expanded={showUsers}
          aria-controls="connected-users-list"
          title="Toggle connected users list"
        >
          {
            showUsers ?
            `Hide connected users (${userList?.length ?? 0})`
            :
            `Show connected users (${userList?.length ?? 0})`
          }
        </button>
        
        {showUsers && (
          <ul id="connected-users-list" className="connected-users-list" aria-live="polite">
            {userList && userList.length > 0 ? (
              userList.map((u, idx) => (
                <li key={`${u.username}-${idx}`} className="connected-user-item">
                  <span className="connected-username">{u.username}</span> :{" "}
                  <span className="connected-at">{u.lastActiveTime}</span>
              </li>
            ))
            ) : (
              <li className="connected-user-item">No connected users</li> 
            )}
          </ul>
        )}
        
      </div>
    </div>
  );
};

export default UserSelection;