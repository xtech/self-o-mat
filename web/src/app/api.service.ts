import {Injectable} from '@angular/core';
import {Observable, throwError} from 'rxjs';
import {catchError, map} from 'rxjs/operators';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';
import {LoadingController} from '@ionic/angular';
import {ActionSheetController} from '@ionic/angular';

import {xtech} from './protos/api';

import * as Long from 'long';

@Injectable({
  providedIn: 'root'
})

export class XAPIService {

    isUpdating = false;
    endUpdateingTimerID = null;
    postTimer = null;
    postLoadingController = null;

    constructor(
    	private readonly http: HttpClient,
    	public loadingController: LoadingController,
    	public actionSheetController: ActionSheetController
    ) {}

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
    getList(val): xtech.selfomat.ListSetting {
        if (!this.isList(val)) {
            return null;
        } else {
            return val as xtech.selfomat.ListSetting;
        }
    }

    getCameraSettings(): Observable<xtech.selfomat.CameraSettings> {
        return this.http.get(`${environment.SERVER_URL}/camera_settings`, {responseType: 'arraybuffer'})
            .pipe(map(res => this.parseCameraSettings(res)),
                catchError(this.handleError)
            );
    }

    getBoothSettings(): Observable<xtech.selfomat.BoothSettings> {
        return this.http.get(`${environment.SERVER_URL}/booth_settings`, {responseType: 'arraybuffer'})
            .pipe(map(res => this.parseBoothSettings(res)),
                catchError(this.handleError)
            );
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

    handleError(error): Observable<any> {
        console.error(error);
        return throwError(error || 'Server error');
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

        this.http.post(environment.SERVER_URL + setting['updateUrl'],
            array.buffer.slice(array.byteOffset, array.byteLength + array.byteOffset),
            {headers: {'Content-Type': 'application/x-www-form-urlencoded'}, responseType: 'arraybuffer'})
            .subscribe(data => {}, error => {
                console.log(error);
            });

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

    async post($event, setting) {
        if (setting instanceof xtech.selfomat.PostSetting) {

            if (setting['alert'].length > 0) {
            	const actionSheet = await this.actionSheetController.create({
	            header: setting['alert'],
	            buttons: [{
	            	text: setting['name'],
	            	handler: () => {
	            		this.postWithoutHint(setting);
	            	}
	            }, {
	            	text: 'Cancel',
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
                    alert('Unsupported file!');
                    return;
                }
            }

            if (this.postLoadingController == null) {
                this.postLoadingController = await this.loadingController.create({});
                await this.postLoadingController.present();
            }

            const reader = new FileReader();

            reader.onload = function () {
                const buffer = new Uint8Array(<ArrayBuffer>reader.result);

                const req = new XMLHttpRequest();
                req.open('POST', environment.SERVER_URL + setting['postUrl'], true);
                req.setRequestHeader('content-type', 'blob');
                req.onload = function () {
                    this.postLoadingController.dismiss();
                    this.postLoadingController = null;
                }.bind(this);
                req.onerror = req.onload;
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
        this.http.post(environment.SERVER_URL + '/trigger',
            null,
            {responseType: 'text'})
            .subscribe(data => {}, error => {
                console.log(error);
            });
    }
}
