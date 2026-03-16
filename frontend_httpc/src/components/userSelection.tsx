import React from "react";
import { setInLocalStorage } from "../services/browserStorage";
import "./userSelection.css";

interface UserSelectionProps {
  userName: string;
  setUserName: (name: string) => void;
}

const UserSelection = ({ userName, setUserName }: UserSelectionProps) => {
  const handleChangeName = () => {
    const newName = window.prompt("Enter a new display name:", userName);
    if (newName && newName.trim() !== "") {
      const trimmed = newName.trim();
      setUserName(trimmed);
      setInLocalStorage("username", trimmed);
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