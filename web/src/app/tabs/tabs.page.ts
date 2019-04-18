import { Component } from '@angular/core';
import { XAPIService } from '../api.service';

@Component({
  selector: 'app-tabs',
  templateUrl: 'tabs.page.html',
  styleUrls: ['tabs.page.scss']
})
export class TabsPage {
  constructor(private readonly apiService: XAPIService) {
  }

  onTouchFAB($event) {
    this.apiService.trigger();
  }
}
