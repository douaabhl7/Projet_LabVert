function updateData() {
  fetch("/data")
    .then(r => r.json())
    .then(d => {
      document.getElementById("temp").innerText = d.temperature + " °C";
      document.getElementById("hum").innerText = d.humidity + " %";
    })
    .catch(err => console.log("Erreur:", err));
}

updateData();
setInterval(updateData, 5000);