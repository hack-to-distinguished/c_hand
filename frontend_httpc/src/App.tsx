import "./App.css";
import MessageDisplay from "./pages/messageDisplay.tsx";

function App() {
  return (
    <div className="app-shell">
      <header className="head_title">
        {/* <span className="head_title-label"></span> */}
        <h1>CORP CHAT</h1>
      </header>
      <div className="main-app-container">
        <MessageDisplay />
      </div>
    </div>
  );
}

export default App;
