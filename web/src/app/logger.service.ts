import {Injectable} from '@angular/core';

@Injectable({
    providedIn: 'root'
})

/*
    See: https://firebase.google.com/docs/analytics/android/webview
 */

export class XLoggerService {

    properties = {};

    constructor() {}

    logEvent(name, params = null) {
        if (!name) {
            return;
        }

        const win = (window as any);

        if (win.AnalyticsWebInterface) {
            // Call Android interface
            win.AnalyticsWebInterface.logEvent(name, params ? JSON.stringify(params) : '');
        } else if (win.webkit
            && win.webkit.messageHandlers
            && win.webkit.messageHandlers.firebase) {
            // Call iOS interface
            const message = {
                command: 'logEvent',
                name: name,
                parameters: params
            };
            win.webkit.messageHandlers.firebase.postMessage(message);
        }
    }

    setUserProperty(name, value) {
        if (!name || !value) {
            return;
        }

        // Only log new properties
        if (this.properties[name] === value) {
            return;
        } else {
            this.properties[name] = value;
        }

        const win = (window as any);

        if (win.AnalyticsWebInterface) {
            // Call Android interface
            win.AnalyticsWebInterface.setUserProperty(name, value);
        } else if (win.webkit
            && win.webkit.messageHandlers
            && win.webkit.messageHandlers.firebase) {
            // Call iOS interface
            const message = {
                command: 'setUserProperty',
                name: name,
                value: value
            };
            win.webkit.messageHandlers.firebase.postMessage(message);
        }
    }
}
