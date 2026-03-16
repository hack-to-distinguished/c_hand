import React, { useState, useEffect, useRef } from "react";
import { setInLocalStorage } from "../services/browserStorage";
import "./userSelection.css";

interface UserSelectionProps {
  userName: string;
  setUserName: (name: string) => void;
}

const UserSelection = ({ userName, setUserName }: UserSelectionProps) => {
  const [isEditing, setIsEditing] = useState(false);
  const [editValue, setEditValue] = useState(userName);
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

  const save = () => {
    const trimmed = editValue.trim();
    if (trimmed && trimmed !== userName) {
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
    </div>
  );
};

export default UserSelection;