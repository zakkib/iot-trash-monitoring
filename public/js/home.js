fetch("../json/device.json")
    .then(res => res.json())
    .then(data => {
        const list = document.getElementById("device-list");

        data.devices.forEach(item => {
            const card = `
                <div class="device-card" onclick="openDevice(${item.id})">
                    <h3>${item.name}</h3>
                    <p>Lokasi: ${item.location}</p>
                    <p>Status: ${item.status}</p>
                    <p>Tinggi Sampah: ${item.height} cm</p>
                    <p>Battery: ${item.battery}%</p>
                </div>
            `;

            list.innerHTML += card;
        });
    });

function openDevice(id) {
    window.location.href = `device.html?id=${id}`;
}