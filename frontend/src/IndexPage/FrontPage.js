import React, { useState, useEffect } from "react";
import socketOBJ from "../Socket/SocketIO.js";
import Card from "react-bootstrap/Card";
import { useNavigate } from "react-router-dom";
import "./FrontPage.css";

function FrontPage(props) {
  const navigate = useNavigate();
  const [timeLeft, setTimeLeft] = useState("");

  useEffect(() => {
    const interval = setInterval(() => {
      const currentTime = Date.now();
      if (props.expiry - currentTime > 0) {
        const minutesLeft = parseInt((props.expiry - currentTime) / 1000 / 60);
        const secondsLeft = parseInt(
          (props.expiry - currentTime) / 1000 - minutesLeft * 60
        );
        setTimeLeft(minutesLeft + "m " + secondsLeft + "s");
      }
    }, 1000);
    return () => clearInterval(interval);
  }, []);

  function emitReq(button) {
    const data = { mode: button };
    socketOBJ.emit("buttonPressed", data);
  }

  return (
    <>
      <Card
        style={{
          width: "50rem",
          boxShadow:
            "inset 0 0 50px #fff,inset 20px 0 80px #f0f,inset -20px 0 80px #0ff,inset 20px 0 300px #f0f,inset -20px 0 300px #0ff",
        }}
      >
        <Card.Body>
          <Card.Title>{"Time Left " + timeLeft}</Card.Title>
        </Card.Body>
      </Card>

      <div className="forminput">
        <button
          value="Default"
          className="fButton"
          onClick={() => emitReq("default")}
        >
          Default
        </button>
      </div>
      <div class="forminput">
        <button value="Theme" class="fButton" onClick={() => emitReq("theme")}>
          {timeLeft}
        </button>
      </div>
      <div className="forminput">
        <button
          value="Custom"
          className="fButton"
          onClick={() => navigate("/control/rgb")}
        >
          Custom
        </button>
      </div>
    </>
  );
}
export default FrontPage;
