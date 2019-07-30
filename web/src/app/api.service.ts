import {Injectable} from '@angular/core';
import {Observable, throwError} from 'rxjs';
import {catchError, map} from 'rxjs/operators';
import {HttpClient, HttpHeaders} from '@angular/common/http';
import {environment} from '../environments/environment';
import {LoadingController} from '@ionic/angular';
import {ActionSheetController} from '@ionic/angular';
import {AlertController} from '@ionic/angular';

import { TranslateService } from '@ngx-translate/core';

import {xtech} from './protos/api';

import * as Long from 'long';
import BoothError = xtech.selfomat.BoothError;

@Injectable({
    providedIn: 'root'
})

export class XAPIService {

    isDemo = (window.location.hostname.indexOf('demo.self-o-mat.de') > -1);

    isUpdating = false;
    endUpdateingTimerID = null;
    postTimer = null;
    postLoadingController = null;
    headers = null;

    constructor(
        private translate: TranslateService,
        private readonly http: HttpClient,
        public loadingController: LoadingController,
        public actionSheetController: ActionSheetController,
        public alertController: AlertController
    ) {

        this.headers = new HttpHeaders({
            'lang': this.translate.currentLang
        });

    }

    values(obj: Object): any[] {
        if (obj == null) {
            return [];
        }

        return Object.entries(obj)
            .filter(([name, value]) => value !== null)
            .map(([name, value]) => (value));

    }

    isBoolean(val) { return typeof val === 'boolean'; }
    isNumber(val) { return typeof val === 'number'; }
    isList(val) { return val instanceof xtech.selfomat.ListSetting; }
    isReadOnly(val) { return val instanceof xtech.selfomat.ReadOnlySetting; }
    isPost(val) { return val instanceof xtech.selfomat.PostSetting; }
    isFileUpload(val) { return val instanceof xtech.selfomat.FileUploadSetting; }
    isLink(val) { return val instanceof xtech.selfomat.LinkSetting; }

    getList(val): xtech.selfomat.ListSetting {
        if (!this.isList(val)) {
            return null;
        } else {
            return val as xtech.selfomat.ListSetting;
        }
    }

    getCameraSettings(): Observable<xtech.selfomat.CameraSettings> {
        if (this.isDemo) {

            const result = new Observable<xtech.selfomat.CameraSettings>((observer) => {
                const settings = new xtech.selfomat.CameraSettings();

                settings.iso = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.CAMERA.iso'),
                    values: ['Auto', '100', '125', '160', '200', '250', '320', '400', '500', '640', '800', '1000'],
                    currentIndex: 0,
                    updateUrl: null
                });

                settings.aperture = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.CAMERA.aperture'),
                    values: ['1.8', '2', '2.2', '2.5', '2.8', '3.2', '3.5', '4', '4.5', '5', '5.6', '6.3', '7.1', '8', '9', '10', '11', '13', '14', '16', '18', '20', '22'],
                    currentIndex: 6,
                    updateUrl: null
                });

                settings.shutterSpeed = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.CAMERA.shutterSpeed'),
                    values: ['Auto'],
                    currentIndex: 0,
                    updateUrl: null
                });

                settings.exposureCompensation = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.CAMERA.exposureCompensation'),
                    values: ['-5', '-4.6', '-4.3', '-4', '-3.6', '-3.3', '-3', '-2.6', '-2.3', '-2', '-1.6', '-1.3', '-1', '-0.6', '-0.3', '0', '0.3', '0.6', '1', '1.3', '1.6', '2', '2.3', '2.6', '3', '3.3', '3.6', '4', '4.3', '4.6', '5'],
                    currentIndex: 13,
                    updateUrl: null
                });

                settings.imageFormat = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.CAMERA.imageFormat'),
                    values: ['RAW + Tiny JPEG', 'RAW + Medium Fine JPEG', 'RAW + Large Fine JPEG'],
                    currentIndex: 2,
                    updateUrl: null
                });

                settings.cameraName = new xtech.selfomat.ReadOnlySetting({
                    name: this.translate.instant('DEMO.CAMERA.cameraName'),
                    value: 'Canon EOS 5D Mark III'
                });

                settings.lensName = new xtech.selfomat.ReadOnlySetting({
                    name: this.translate.instant('DEMO.CAMERA.lensName'),
                    value: 'EF24-70mm f/4L IS USM'
                });

                settings.focus = new xtech.selfomat.PostSetting({
                    name: this.translate.instant('DEMO.CAMERA.focus'),
                    postUrl: null
                });

                observer.next(settings);
                observer.complete();
            });
            return result;

        } else {
            return this.http.get(`${environment.SERVER_URL}/camera_settings`, {responseType: 'arraybuffer', headers: this.headers} )
                .pipe(map(res => this.parseCameraSettings(res)),
                    catchError(this.handleError)
                );
        }
    }

    getBoothSettings(): Observable<xtech.selfomat.BoothSettings> {
        if (this.isDemo) {

            const result = new Observable<xtech.selfomat.BoothSettings>((observer) => {
                const settings = new xtech.selfomat.BoothSettings();

                settings.storageEnabled = new xtech.selfomat.BoolSetting({
                    name: this.translate.instant('DEMO.BOOTH.storageEnabled'),
                    currentValue: true,
                    updateUrl: null
                });

                settings.printerEnabled = new xtech.selfomat.BoolSetting({
                    name: this.translate.instant('DEMO.BOOTH.printerEnabled'),
                    currentValue: false,
                    updateUrl: null
                });

                settings.flashEnabled = new xtech.selfomat.BoolSetting({
                    name: this.translate.instant('DEMO.BOOTH.flashEnabled'),
                    currentValue: true,
                    updateUrl: null
                });

                settings.flashDurationMicros = new xtech.selfomat.IntSetting({
                    name: this.translate.instant('DEMO.BOOTH.flashDurationMicros'),
                    currentValue: 80,
                    minValue: 0,
                    maxValue: 100,
                    updateUrl: null
                });

                settings.countdownDuration = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.BOOTH.countdownDuration'),
                    values: ['3s', '4s', '5s', '6s', '7s', '8s', '9s', '10s', '15s', '30s'],
                    currentIndex: 2,
                    updateUrl: null
                });

                settings.filterChoice = new xtech.selfomat.ListSetting({
                    name: this.translate.instant('DEMO.BOOTH.filterChoice'),
                    values: ['Off', 'Auto'],
                    currentIndex: 1,
                    updateUrl: null
                });

                settings.filterGain = new xtech.selfomat.FloatSetting({
                    name: this.translate.instant('DEMO.BOOTH.filterGain'),
                    currentValue: 0.5,
                    minValue: 0,
                    maxValue: 1,
                    updateUrl: null
                });

                settings.templateUpload = new xtech.selfomat.FileUploadSetting( {
                    name: this.translate.instant('DEMO.BOOTH.templateUpload'),
                    postUrl: null,
                    inputAccept: 'image/x-png,image/png'
                });

                settings.templateEnabled = new xtech.selfomat.BoolSetting({
                    name: this.translate.instant('DEMO.BOOTH.templateEnabled'),
                    currentValue: false,
                    updateUrl: null
                });

                settings.cupsLink = new xtech.selfomat.LinkSetting( {
                    name: this.translate.instant('DEMO.BOOTH.cupsLink'),
                    url: 'https://www.cups.org'
                })

                settings.updateMode = new xtech.selfomat.PostSetting({
                    name: this.translate.instant('DEMO.BOOTH.updateMode'),
                    postUrl: null,
                    alert: this.translate.instant('DEMO.BOOTH.updateModeAlert')
                });

                observer.next(settings);
                observer.complete();
            });
            return result;

        } else {
            return this.http.get(`${environment.SERVER_URL}/booth_settings`, {responseType: 'arraybuffer', headers: this.headers})
                .pipe(map(res => this.parseBoothSettings(res)),
                    catchError(this.handleError)
                );
        }
    }

    parseCameraSettings(response: ArrayBuffer): xtech.selfomat.CameraSettings {
        const cameraSettings = xtech.selfomat.CameraSettings.decode(new Uint8Array(response));
        this.checkPostTimer();
        return cameraSettings;
    }

    parseBoothSettings(response: ArrayBuffer): xtech.selfomat.BoothSettings {
        const boothSettings = xtech.selfomat.BoothSettings.decode(new Uint8Array(response));
        this.checkPostTimer();
        return boothSettings;
    }

    parseBoothError(response: ArrayBuffer): xtech.selfomat.BoothError {
        const boothError = xtech.selfomat.BoothError.decode(new Uint8Array(response));
        return boothError;
    }


    handleError(error): Observable<any> {
        console.error(error);
        return throwError(error || 'Server error');
    }

    async presentAlert(title, message) {
        const alert = await this.alertController.create({
            header: title,
            subHeader: null,
            message: message,
            buttons: ['OK']
        });

        await alert.present();
    }

    clickItem($event, setting, index) {
        $event.stopPropagation();
        if (this.isFileUpload(setting)) {
            const input = <HTMLInputElement>document.getElementById('input_' + index);
            if (input.value) {
                try {
                    input.value = '';
                    if (input.value) {
                        input.type = 'text';
                        input.type = 'file';
                    }
                } catch (e) {}
            }
            input.click();
        } else if (this.isPost(setting)) {
            this.post($event, setting);
        } else if (this.isLink(setting)) {
            window.location.href = setting['url'];
        }
    }

    updateSetting($event, setting) {

        if (!this.isUpdating) {
            this.isUpdating = true;
            this.endUpdateingTimerID = setTimeout(() => {
                this.isUpdating = false;
                this.endUpdateingTimerID = null;
            }, 5000);
        }

        let array;

        if (setting instanceof xtech.selfomat.BoolSetting) {
            const message = xtech.selfomat.BoolUpdate.create({ value: setting['currentValue'] });
            array =  xtech.selfomat.BoolUpdate.encode(message).finish();
        } else if (setting instanceof xtech.selfomat.FloatSetting) {
            const message = xtech.selfomat.FloatUpdate.create({ value: setting['currentValue'] });
            array =  xtech.selfomat.FloatUpdate.encode(message).finish();
        } else if (setting instanceof xtech.selfomat.IntSetting) {
            const message = xtech.selfomat.IntUpdate.create({ value: setting['currentValue'] as Long });
            array =  xtech.selfomat.IntUpdate.encode(message).finish();
        } else if (setting instanceof xtech.selfomat.ListSetting) {
            console.log('LIST');
            const message = xtech.selfomat.IntUpdate.create({ value: setting['currentIndex'] });
            array =  xtech.selfomat.IntUpdate.encode(message).finish();
        }

        if (!this.isDemo) {
            this.http.post(environment.SERVER_URL + setting['updateUrl'],
                array.buffer.slice(array.byteOffset, array.byteLength + array.byteOffset),
                {headers: {'Content-Type': 'application/x-www-form-urlencoded'}, responseType: 'arraybuffer'})
                .subscribe(data => {}, error => {
                    console.log(error);
                });
        }
    }

    checkPostTimer() {
        if (this.postTimer == null || this.postLoadingController == null) {
            return;
        }

        if (Date.now() - this.postTimer > 10000) {
            this.postLoadingController.dismiss();
            this.postLoadingController = null;
            this.postTimer = null;
        }
    }

    async postWithoutHint(setting) {
        if (setting instanceof xtech.selfomat.PostSetting) {

            if (this.postLoadingController == null) {
                this.postLoadingController = await this.loadingController.create({});
                await this.postLoadingController.present();
            }

            this.postTimer = Date.now();

            if (this.isDemo) {
                this.postLoadingController.dismiss();
                this.postLoadingController = null;
                this.postTimer = null;
            } else {
                this.http.post(environment.SERVER_URL + setting['postUrl'],
                    null,
                    {responseType: 'text'})
                    .subscribe(data => {
                        this.postLoadingController.dismiss();
                        this.postLoadingController = null;
                        this.postTimer = null;
                    }, error => {});
            }
        }
    }

    async post($event, setting) {
        if (this.isPost(setting)) {

            if (setting['alert'].length > 0) {
                const actionSheet = await this.actionSheetController.create({
                    header: setting['alert'],
                    buttons: [{
                        text: setting['name'],
                        handler: () => {
                            this.postWithoutHint(setting);
                        }
                    }, {
                        text: this.translate.instant('CANCEL'),
                        role: 'cancel',
                        handler: () => {}
                    }]
                });

                await actionSheet.present();
            } else {
                this.postWithoutHint(setting);
            }

        }
    }

    async fileUpload($event, setting) {
        if (setting instanceof xtech.selfomat.FileUploadSetting) {

            if ($event.target.files.lengh < 1) {
                return;
            }

            const file = $event.target.files[0];
            if (setting['inputAccept'].length > 0) {
                const types = setting['inputAccept'].split(',');
                if (!types.includes(file.type.toLowerCase())) {
                    this.presentAlert(this.translate.instant('ERROR'), this.translate.instant('ERRORS.fileUnsupported'));
                    return;
                }
            }

            if (this.postLoadingController == null) {
                this.postLoadingController = await this.loadingController.create({});
                await this.postLoadingController.present();
            }

            const reader = new FileReader();

            reader.onload = function () {
                if (this.isDemo) {
                    this.postLoadingController.dismiss();
                    this.postLoadingController = null;
                    return;
                }

                const buffer = new Uint8Array(<ArrayBuffer>reader.result);

                const req = new XMLHttpRequest();
                req.open('POST', environment.SERVER_URL + setting['postUrl'], true);
                req.setRequestHeader('content-type', 'blob');
                req.responseType = 'arraybuffer';
                req.onreadystatechange = function() {
                    if (req.readyState === XMLHttpRequest.DONE) {
                        this.postLoadingController.dismiss();
                        this.postLoadingController = null;

                        try {
                            const error = this.parseBoothError(req.response);
                            if (error.code > 0) {
                                this.presentAlert(error.title, error.message);
                            }
                        } catch (e) {}
                    }
                }.bind(this);

                req.onerror = req.onreadystatechange;
                req.send(buffer);

            }.bind(this);

            reader.onerror = function () {
                this.postLoadingController.dismiss();
                this.postLoadingController = null;
            }.bind(this);

            reader.readAsArrayBuffer(file);
        }
    }

    trigger() {
        if (!this.isDemo) {
            this.http.post(environment.SERVER_URL + '/trigger',
                null,
                {responseType: 'text'})
                .subscribe(data => {}, error => {
                    console.log(error);
                });
        }
    }
}
