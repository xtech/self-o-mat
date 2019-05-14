import { Component, OnInit } from '@angular/core';
import { XAPIService } from '../api.service';
import {xtech} from '../protos/api';

import BoothSettings = xtech.selfomat.BoothSettings;

@Component({
  selector: 'app-tab2',
  templateUrl: '../settings.page.html',
  styleUrls: ['../settings.page.scss']
})

export class Tab2Page implements OnInit {

  settings: BoothSettings;
  pageTitle = 'Self-o-mat Settings';

  constructor(readonly apiService: XAPIService) {
  }

  updateBoothSettings(boothSettings: BoothSettings) {
    if (this.apiService.isUpdating) {
      return;
    }

    if (this.settings == null) {
      this.settings = boothSettings;
      return;
    }

    const array1 =  xtech.selfomat.BoothSettings.encode(this.settings).finish();
    const array2 =  xtech.selfomat.BoothSettings.encode(boothSettings).finish();

    if (array1.toString() !== array2.toString()) {
      this.settings = boothSettings;
    }
  }

  refresh() {
    if (this.apiService.isUpdating) return;
    this.apiService.getBoothSettings().subscribe(data => this.updateBoothSettings(data));
  }

  ngOnInit() {
    setInterval(() => {
      this.refresh();
    }, 1000);

    this.refresh();
  }
}
