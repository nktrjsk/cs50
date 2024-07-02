"use strict";

const jar = Math.floor(Math.random() * 3);
const gameStatus = document.querySelector("#game-status");

// Reference: https://stackoverflow.com/questions/4950115/removeeventlistener-on-anonymous-functions-in-javascript
const controller = new AbortController();
document.querySelectorAll("#game p").forEach(function (element, index) {
    element.addEventListener("click", function (event) {
        if (index == jar) {
            element.innerText = "🔴";
            gameStatus.innerText = "Nice!";
        } else {
            element.innerText = "🤡";
            gameStatus.innerText = "Next time";
        }

        document.querySelectorAll("#game p").forEach(function (element) {
            element.style.cursor = "default";
        });
        controller.abort()
    }, {signal: controller.signal});
});