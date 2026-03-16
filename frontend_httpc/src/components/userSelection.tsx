import React, { useState, useEffect } from "react";
import { generateId } from "../services/generateRandom";
import { setInLocalStorage } from "../services/browserStorage";
import "./userSelection.css";

const UserSelection = () => {
  const [userName, setUserName] = useState("");

  useEffect(() => {
    if (userName) return;

    let username = localStorage.getItem("username");
    if (!username) {
      username = generateId(5);
      setInLocalStorage("username", username);
    }
    setUserName(username || "");
  }, [userName]);

  const handleChangeName = () => {
    const newName = window.prompt("Enter a new display name:", userName);
    if (newName && newName.trim() !== "") {
      setUserName(newName.trim());
      setInLocalStorage("username", newName.trim());
    }
  };

  const handleKeyDown = (e: React.KeyboardEvent<HTMLSpanElement>) => {
    if (e.key === "Enter" || e.key === " ") {
      e.preventDefault();
      handleChangeName();
    }
  };

  return (
    <div className="user-selection">
      <div className="user-selection-row">
        <strong>User:</strong>
        <span
          className="user-name"
          role="button"
          tabIndex={0}
          onClick={handleChangeName}
          onKeyDown={handleKeyDown}
          aria-label="Change display name"
          title="Click or press Enter to change name"
        >
          {userName}
        </span>
      </div>
    </div>
  );
};

export default UserSelection;