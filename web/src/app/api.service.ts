import {Injectable} from '@angular/core';
import {Observable, throwError} from 'rxjs';
import {catchError, map} from 'rxjs/operators';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';
import {LoadingController} from '@ionic/angular';

import {xtech} from './protos/api';

import * as Long from 'long';

@Injectable({
  providedIn: 'root'
})

export class XAPIService {

    isUpdating: boolean = false;
    stopUpdating: boolean = false;

    constructor(
    	private readonly http: HttpClient,
    	public loadingController: LoadingController
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
        return cameraSettings;
    }

    parseBoothSettings(response: ArrayBuffer): xtech.selfomat.BoothSettings {
        const boothSettings = xtech.selfomat.BoothSettings.decode(new Uint8Array(response));
        return boothSettings;
    }

    handleError(error): Observable<any> {
        console.error(error);
        return throwError(error || 'Server error');
    }

    endUpdating($event) {
        this.stopUpdating = true;
    }

    updateSetting($event, setting) {

	if (!this.isUpdating) {
            this.isUpdating = true;
            this.stopUpdating = false;
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
            .subscribe(data => {
		if (this.stopUpdating) {
			this.isUpdating = false;
		}
            }, error => {
		if (this.stopUpdating) {
			this.isUpdating = false;
		}
                console.log(error);
            });

    }

    async post($event, setting) {
        if (setting instanceof xtech.selfomat.PostSetting) {

            const loading = await this.loadingController.create({});
            await loading.present();

            this.http.post(environment.SERVER_URL + setting['postUrl'],
            	null,
            	{responseType: 'text'})
            	.subscribe(data => {
             	   loading.dismiss();
            	}, error => {
             	   console.log(error);
            	});
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
