const submitButton = document.querySelector("#submit")
// Reference: https://stackoverflow.com/questions/149055/how-to-format-numbers-as-currency-strings
const usd = new Intl.NumberFormat("en-US", {style: "currency", currency: "USD"});
buystatus = false;

submitButton.addEventListener("click", function (event) {
    const symbolField = document.querySelector("#symbol");
    const quantityInput = document.querySelector("#quantity");

    if (symbolField.value == "")
        return;

    if (!buystatus) {
        const data = fetch("/lookup?symbol=" + symbolField.value)
        .then((response) => response.json())
        .then((data) => {
            document.querySelector("#symbol").disabled = true;
            quantityInput.type = "number";
            document.querySelector("#name").innerText = data.name;
            const price = document.querySelector("#price");
            price.innerText = usd.format(data.price);
            quantityInput.addEventListener("change", function (event) {
                price.innerText = usd.format(Number(quantityInput.value) * data.price);
            });

            submitButton.innerText = "Buy";

            document.querySelector("#info").style.display = "unset";

            buystatus = true;
        });
    } else {
        let data = new FormData();
        data.append("symbol", symbolField.value);
        data.append("shares", quantityInput.value);

        fetch("/buy", {
            method: "POST",
            body: data
            }
        );
    }
});