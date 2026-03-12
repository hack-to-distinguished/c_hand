import { useState, useEffect } from "react";
import { generateId } from "../services/generateRandom";
import { setInLocalStorage } from "../services/browserStorage";


const UserSelection = () => {
  const [userName, setUserName] = useState("");
  // display on the side
  // give the option to the user to change that name
  // check against existing names
  // Save new name

  
  useEffect(() => {
    if (userName) { return };
    
    let username = localStorage.getItem("username");
    if (!username) {
      username = generateId(5);
      setInLocalStorage("username", username);
    }
    setUserName(username);
    
  }, [setUserName, userName]);
  
  return (
    <div >
      `Username is ${userName}`
    </div>
  );
};

export default UserSelection;