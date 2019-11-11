import { Component } from '@angular/core';
import {HttpClient} from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'gui';

  cardsData: JSON;

  // INSTALL CORS PACKAGE ???
  constructor(private httpClient: HttpClient) {
    console.log('bleble');
    this.getAllCards();
  }

  getAllCards() {
    this.httpClient.get('http://127.0.0.1:5000/cards').subscribe(data => {
      this.cardsData = data as JSON;
      console.log(this.cardsData);
    });
  }
}
