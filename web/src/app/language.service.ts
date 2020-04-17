import { TranslateService } from '@ngx-translate/core';
import { Injectable } from '@angular/core';


@Injectable({
  providedIn: 'root'
})
export class LanguageService {

  constructor(private translate: TranslateService) {
    const language = this.translate.getBrowserLang();
    this.translate.setDefaultLang('en');
    this.translate.use(language);
  }
}
