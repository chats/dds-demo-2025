class MessageHandler {
    constructor(containerId, currentFilters) {
        this.messageContainer = document.getElementById(containerId);
        this.currentFilters = currentFilters;
        this.setupEventSource();
    }

    setupEventSource() {
        const evtSource = new EventSource("/stream");
        
        evtSource.onmessage = (event) => {
            const message = JSON.parse(event.data);
            const card = this.createMessageCard(message);
            if (card) {
                this.messageContainer.insertBefore(card, this.messageContainer.firstChild);
            }
        };

        evtSource.onerror = (err) => {
            console.error("EventSource failed:", err);
        };
    }

    shouldShowMessage(recipient) {
        return this.currentFilters.includes('ALL') || 
               this.currentFilters.includes('*') || 
               this.currentFilters.includes(recipient);
    }

    createExcelTable(excelData) {
        if (!excelData) return '';
        
        const headers = excelData.headers.map(h => `<th>${this.escapeHtml(h)}</th>`).join('');
        const rows = excelData.rows.map(row => 
            `<tr>${row.map(cell => `<td>${this.escapeHtml(cell)}</td>`).join('')}</tr>`
        ).join('');

        return `
            <div class="excel-data">
                <table class="excel-table">
                    <thead><tr>${headers}</tr></thead>
                    <tbody>${rows}</tbody>
                </table>
            </div>
        `;
    }

    escapeHtml(unsafe) {
        return unsafe
            ? String(unsafe)
                .replace(/&/g, "&amp;")
                .replace(/</g, "&lt;")
                .replace(/>/g, "&gt;")
                .replace(/"/g, "&quot;")
                .replace(/'/g, "&#039;")
            : '';
    }

    createMessageCard(message) {
        if (!this.shouldShowMessage(message.recipient)) {
            return null;
        }

        const card = document.createElement('div');
        card.className = 'message-card new-message';

        const excelTable = this.createExcelTable(message.excel_data);

        card.innerHTML = `
            <div class="message-header">
                <h3>${this.escapeHtml(message.subject)}</h3>
                <div class="message-meta">
                    Recipient: ${this.escapeHtml(message.recipient)}<br>
                    Time: ${this.escapeHtml(message.timestamp)}
                </div>
            </div>
            ${excelTable}
        `;

        return card;
    }
}

document.addEventListener('DOMContentLoaded', () => {
    new MessageHandler('messageContainer', currentFilters);
});