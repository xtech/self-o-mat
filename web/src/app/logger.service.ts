import {Injectable} from '@angular/core';

@Injectable({
    providedIn: 'root'
})

export class XLoggerService {

    constructor() {}

    logEvent(name, params = null) {
        if (!name) {
            return;
        }

        console.log(name + ' ' + params);

        if ('AnalyticsWebInterface' in window) {
            window!['AnalyticsWebInterface']!.logEvent(name, params ? JSON.stringify(params) : '');
        } else if ('webkit' in window
            && 'messageHandlers' in window['webkit']
            && 'firebase' in window['webkit']['messageHandlers']) {
            // Call iOS interface
            const message = {
                command: 'logEvent',
                name: name,
                parameters: params
            };
            window!['webkit']['messageHandlers']['firebase']!.postMessage(message);
        } else {
            // No Android or iOS interface found
            console.log('No native APIs found.');
        }


    }

    setUserProperty(name, value) {
        if (!name || !value) {
            return;
        }

        if ('AnalyticsWebInterface' in window) {
            window!['AnalyticsWebInterface']!.setUserProperty(name, value);
        } else if ('webkit' in window
            && 'messageHandlers' in window['webkit']
            && 'firebase' in window['webkit']['messageHandlers']) {
            // Call iOS interface
            const message = {
                command: 'logEvent',
                name: name,
                value: value
            };
            window!['webkit']['messageHandlers']['firebase']!.postMessage(message);
        } else {
            // No Android or iOS interface found
            console.log('No native APIs found.');
        }

    }
}
