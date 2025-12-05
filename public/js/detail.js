// DATA DUMMY (bisa diganti dari API) //bisa pake jsonnya harusnya
const historyData = [
    {
        status: "KOSONG",
        tinggi: "22 cm",
        battery: "90%",
        date: "2025-11-20 10:21"
    },
    {
        status: "PENUH",
        tinggi: "3 cm",
        battery: "87%",
        date: "2025-11-20 09:10"
    },

    {
        status: "PENUH",
        tinggi: "4 cm",
        battery: "85%",
        date: "2025-11-19 17:10"
    }
];

// Render Data Historis
function renderHistory() {
    const list = document.getElementById("historyList");
    list.innerHTML = ""; 

    historyData.forEach(item => {
        const row = document.createElement("div");
        row.classList.add("history-row");

        row.innerHTML = `
            <span>${item.status}</span>
            <span>${item.tinggi}</span>
            <span>${item.battery}</span>
            <span>${item.date}</span>
        `;

        list.appendChild(row);
    });
}

renderHistory();


// Contoh update data (bisa dipanggil oleh API real-time nanti)
function updateDeviceInfo(status, tinggi, battery) {
    document.getElementById("statusValue").innerText = status;
    document.getElementById("tinggiValue").innerText = tinggi + " cm";
    document.getElementById("batteryValue").innerText = battery + "%";

    historyData.unshift({
        status,
        tinggi: tinggi + " cm",
        battery: battery + "%",
        date: new Date().toLocaleString()
    });

    renderHistory();
}

// CONTOH MENAMBAHKAN DATA BARU OTOMATIS
// setTimeout(() => {
//     updateDeviceInfo("PENUH", 5, 80);
// }, 3000);