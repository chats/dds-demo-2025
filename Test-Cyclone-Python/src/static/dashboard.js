// Dashboard state
const state = {
    devices: {},         // Store data for charts
    stats: {},          // Store statistics
    latestData: {},     // Store latest values
    lastValues: {},     // Store previous values for trend
    charts: {},         // Store Chart.js instances
    isConnected: false, // WebSocket connection status
    timeFilter: {
        start: null,
        end: null
    }
};

const MAX_DATA_POINTS = 20;

// Threshold configurations
const THRESHOLDS = {
    temperature: {
        min: 20,
        max: 30,
        unit: '°C'
    },
    humidity: {
        min: 30,
        max: 70,
        unit: '%'
    }
};

// Initialize date pickers
function initializeDatePickers() {
    const commonConfig = {
        enableTime: true,
        dateFormat: "Y-m-d H:i",
        time_24hr: true,
        onChange: updateAllCharts
    };

    flatpickr("#startTime", {
        ...commonConfig,
        onChange: ([date]) => {
            state.timeFilter.start = date;
            updateAllCharts();
        }
    });

    flatpickr("#endTime", {
        ...commonConfig,
        onChange: ([date]) => {
            state.timeFilter.end = date;
            updateAllCharts();
        }
    });
}

// Update connection status UI
function updateConnectionStatus(connected) {
    const indicator = document.getElementById('connectionStatus');
    const text = document.getElementById('connectionText');
    
    state.isConnected = connected;
    indicator.className = `status-indicator status-${connected ? 'active' : 'inactive'}`;
    text.textContent = connected ? 'Connected' : 'Disconnected';
}

// Get value trend (up/down/none)
function getValueTrend(deviceId, field, currentValue) {
    const lastData = state.lastValues[deviceId];
    if (!lastData) return '';
    
    const lastValue = lastData[field];
    if (currentValue > lastValue) return 'value-up';
    if (currentValue < lastValue) return 'value-down';
    return '';
}

// Create or update sensor card
function updateSensorCard(deviceId, data) {
    let card = document.getElementById(`card-${deviceId}`);
    const cardsContainer = document.getElementById('sensorCards');

    if (!card) {
        card = document.createElement('div');
        card.id = `card-${deviceId}`;
        card.className = 'sensor-card bg-white rounded-lg p-4 shadow-md';
        cardsContainer.appendChild(card);
    }

    // Get value trends
    const tempTrend = getValueTrend(deviceId, 'temperature', data.temperature);
    const humidTrend = getValueTrend(deviceId, 'humidity', data.humidity);

    card.innerHTML = `
        <div class="flex items-center justify-between mb-2">
            <h2 class="text-lg font-semibold">Device: ${deviceId}</h2>
        </div>
        <div class="space-y-2">
            <div class="bg-purple-50 px-3 py-2 rounded">
                Temperature: 
                <span class="${tempTrend}">${data.temperature.toFixed(1)}°C</span>
            </div>
            <div class="bg-green-50 px-3 py-2 rounded">
                Humidity: 
                <span class="${humidTrend}">${data.humidity.toFixed(1)}%</span>
            </div>
            <p class="text-sm text-gray-500">
                Last Update: ${new Date(data.timestamp * 1000).toLocaleTimeString()}
            </p>
        </div>
    `;

    // Check thresholds and show alerts if needed
    checkThresholds(deviceId, data);
}

// Check thresholds and show alerts
function checkThresholds(deviceId, data) {
    const alerts = [];
    
    if (data.temperature > THRESHOLDS.temperature.max) {
        alerts.push(`High Temperature Alert: ${data.temperature}°C (Threshold: ${THRESHOLDS.temperature.max}°C)`);
    }
    if (data.temperature < THRESHOLDS.temperature.min) {
        alerts.push(`Low Temperature Alert: ${data.temperature}°C (Threshold: ${THRESHOLDS.temperature.min}°C)`);
    }
    if (data.humidity > THRESHOLDS.humidity.max) {
        alerts.push(`High Humidity Alert: ${data.humidity}% (Threshold: ${THRESHOLDS.humidity.max}%)`);
    }
    if (data.humidity < THRESHOLDS.humidity.min) {
        alerts.push(`Low Humidity Alert: ${data.humidity}% (Threshold: ${THRESHOLDS.humidity.min}%)`);
    }

    if (alerts.length > 0) {
        showAlerts(deviceId, alerts);
    }
}

// Show alerts in UI
function showAlerts(deviceId, alerts) {
    const alertsContainer = document.getElementById('alertsContainer');
    alerts.forEach(alert => {
        const alertDiv = document.createElement('div');
        alertDiv.className = 'alert';
        alertDiv.innerHTML = `
            <div class="flex justify-between items-center">
                <div>
                    <strong>Device ${deviceId}:</strong> ${alert}
                </div>
                <button onclick="this.parentElement.parentElement.remove()" 
                        class="text-gray-500 hover:text-gray-700">
                    ×
                </button>
            </div>
        `;
        alertsContainer.insertBefore(alertDiv, alertsContainer.firstChild);

        // Remove alert after 10 seconds
        setTimeout(() => alertDiv.remove(), 10000);
    });
}

// Create or update charts
function updateChart(deviceId, data) {
    const tempChartId = `temp-chart-${deviceId}`;
    const humidChartId = `humid-chart-${deviceId}`;
    let tempContainer = document.getElementById(`${tempChartId}-container`);
    let humidContainer = document.getElementById(`${humidChartId}-container`);
    const chartsContainer = document.getElementById('chartContainers');

    // Create temperature chart if it doesn't exist
    if (!tempContainer) {
        tempContainer = createChartContainer(deviceId, tempChartId, 'Temperature');
        chartsContainer.appendChild(tempContainer);
        initializeChart(tempChartId, 'Temperature (°C)', '#8884d8');
    }

    // Create humidity chart if it doesn't exist
    if (!humidContainer) {
        humidContainer = createChartContainer(deviceId, humidChartId, 'Humidity');
        chartsContainer.appendChild(humidContainer);
        initializeChart(humidChartId, 'Humidity (%)', '#82ca9d');
    }

    // Update charts if within time filter
    const timestamp = new Date(data.timestamp * 1000);
    if (isWithinTimeFilter(timestamp)) {
        updateChartData(tempChartId, timestamp, data.temperature);
        updateChartData(humidChartId, timestamp, data.humidity);
    }
}

// Create chart container
function createChartContainer(deviceId, chartId, title) {
    const container = document.createElement('div');
    container.id = `${chartId}-container`;
    container.className = 'bg-white p-4 rounded-lg shadow mb-4';
    container.innerHTML = `
        <h2 class="text-lg font-semibold mb-2">Device ${deviceId} - ${title} History</h2>
        <div style="height: 350px;">
            <canvas id="${chartId}"></canvas>
        </div>
    `;
    return container;
}

// Initialize new chart
function initializeChart(chartId, label, color) {
    const ctx = document.getElementById(chartId).getContext('2d');
    state.charts[chartId] = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: label,
                data: [],
                borderColor: color,
                fill: false,
                tension: 0.4
            }]
        },
        options: {
            responsive: true,
            plugins: {
                title: { display: true, text: label },
                tooltip: {
                    mode: 'index',
                    intersect: false,
                }
            },
            scales: {
                y: {
                    title: { 
                        display: true, 
                        text: label 
                    },
                    beginAtZero: false
                }
            }
        }
    });
}

// Update chart data
function updateChartData(chartId, timestamp, value) {
    const chart = state.charts[chartId];
    chart.data.labels.push(timestamp.toLocaleTimeString());
    chart.data.datasets[0].data.push(value);

    if (chart.data.labels.length > MAX_DATA_POINTS) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
    }

    chart.update();
}

// Update statistics
function updateStats(deviceId, data) {
    if (!state.stats[deviceId]) {
        state.stats[deviceId] = {
            temp_min: data.temperature,
            temp_max: data.temperature,
            temp_sum: 0,
            humid_min: data.humidity,
            humid_max: data.humidity,
            humid_sum: 0,
            count: 0
        };
    }

    const stats = state.stats[deviceId];
    stats.temp_min = Math.min(stats.temp_min, data.temperature);
    stats.temp_max = Math.max(stats.temp_max, data.temperature);
    stats.temp_sum += data.temperature;
    stats.humid_min = Math.min(stats.humid_min, data.humidity);
    stats.humid_max = Math.max(stats.humid_max, data.humidity);
    stats.humid_sum += data.humidity;
    stats.count++;

    updateStatsUI(deviceId, stats);
}

// Update statistics UI
function updateStatsUI(deviceId, stats) {
    let statsCard = document.getElementById(`stats-${deviceId}`);
    const statsContainer = document.getElementById('statsGrid');

    if (!statsCard) {
        statsCard = document.createElement('div');
        statsCard.id = `stats-${deviceId}`;
        statsCard.className = 'bg-white p-4 rounded-lg shadow';
        statsContainer.appendChild(statsCard);
    }

    const tempAvg = stats.temp_sum / stats.count;
    const humidAvg = stats.humid_sum / stats.count;

    statsCard.innerHTML = `
        <h2 class="text-lg font-semibold mb-4">Device ${deviceId} Statistics</h2>
        <div class="grid grid-cols-2 gap-4">
            <div>
                <h3 class="font-medium text-purple-600">Temperature</h3>
                <p>Min: ${stats.temp_min.toFixed(1)}°C</p>
                <p>Max: ${stats.temp_max.toFixed(1)}°C</p>
                <p>Average: ${tempAvg.toFixed(1)}°C</p>
            </div>
            <div>
                <h3 class="font-medium text-green-600">Humidity</h3>
                <p>Min: ${stats.humid_min.toFixed(1)}%</p>
                <p>Max: ${stats.humid_max.toFixed(1)}%</p>
                <p>Average: ${humidAvg.toFixed(1)}%</p>
            </div>
        </div>
    `;
}

// Check if timestamp is within selected time filter
function isWithinTimeFilter(timestamp) {
    if (!state.timeFilter.start && !state.timeFilter.end) return true;
    if (state.timeFilter.start && timestamp < state.timeFilter.start) return false;
    if (state.timeFilter.end && timestamp > state.timeFilter.end) return false;
    return true;
}

// Update all charts based on time filter
function updateAllCharts() {
    Object.entries(state.devices).forEach(([deviceId, data]) => {
        // Clear existing chart data
        const tempChartId = `temp-chart-${deviceId}`;
        const humidChartId = `humid-chart-${deviceId}`;
        
        if (state.charts[tempChartId]) {
            state.charts[tempChartId].data.labels = [];
            state.charts[tempChartId].data.datasets[0].data = [];
            state.charts[tempChartId].update();
        }
        
        if (state.charts[humidChartId]) {
            state.charts[humidChartId].data.labels = [];
            state.charts[humidChartId].data.datasets[0].data = [];
            state.charts[humidChartId].update();
        }

        // Reapply filtered data
        data.forEach(point => {
            const timestamp = new Date(point.timestamp * 1000);
            if (isWithinTimeFilter(timestamp)) {
                updateChart(deviceId, point);
            }
        });
    });
}

// Process new data from WebSocket
function processData(data) {
    const { device_id, temperature, humidity, timestamp } = data;

    // Store data for time filtering
    if (!state.devices[device_id]) {
        state.devices[device_id] = [];
    }
    state.devices[device_id].push(data);
    if (state.devices[device_id].length > MAX_DATA_POINTS * 2) {
        state.devices[device_id].shift();
    }

    // Update latest and last values
    state.lastValues[device_id] = state.latestData[device_id];
    state.latestData[device_id] = { temperature, humidity, timestamp };

    // Update UI components
    updateSensorCard(device_id, data);
    updateChart(device_id, data);
    updateStats(device_id, data);
}

// Setup WebSocket connection
function connectWebSocket() {
    const wsUrl = `ws://${window.location.host}/ws`;
    const ws = new WebSocket(wsUrl);

    ws.onopen = () => {
        console.log('WebSocket Connected');
        updateConnectionStatus(true);
    };

    ws.onclose = () => {
        console.log('WebSocket Disconnected');
        updateConnectionStatus(false);
        setTimeout(connectWebSocket, 5000);
    };

    ws.onmessage = (event) => {
        try {
            const data = JSON.parse(event.data);
            processData(data);
        } catch (error) {
            console.error('Error processing data:', error);
        }
    };

    ws.onerror = (error) => {
        console.error('WebSocket Error:', error);
    };
}

// Initialize dashboard
function initializeDashboard() {
    initializeDatePickers();
    connectWebSocket();
}

// Start the dashboard when page loads
document.addEventListener('DOMContentLoaded', initializeDashboard);