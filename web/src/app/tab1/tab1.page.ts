import { Component, OnInit } from '@angular/core';
import { XAPIService } from '../api.service';
import {xtech} from '../protos/api';

import CameraSettings = xtech.selfomat.CameraSettings;

@Component({
  selector: 'app-tab1',
  templateUrl: '../settings.page.html',
  styleUrls: ['../settings.page.scss']
})

export class Tab1Page implements OnInit {

  settings: CameraSettings;
  pageTitle = 'Camera Settings';

  constructor(private readonly apiService: XAPIService) {
  }

  updateCameraSettings(cameraSettings: CameraSettings) {
    if (this.settings == null) {
      this.settings = cameraSettings;
      return;
    }

    const array1 =  xtech.selfomat.CameraSettings.encode(this.settings).finish();
    const array2 =  xtech.selfomat.CameraSettings.encode(cameraSettings).finish();

    if (array1.toString() !== array2.toString()) {
      this.settings = cameraSettings;
    }
  }

  refresh() {
    this.apiService.getCameraSettings().subscribe(data => this.updateCameraSettings(data));
  }

  ngOnInit() {
    setInterval(() => {
      this.refresh();
    }, 1000);

    this.refresh();
  }
}
