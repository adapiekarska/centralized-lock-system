import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';
import {Observable} from 'rxjs';

@Injectable()
export class AccessService {

  API_URL = environment.API_URL;
  URI = 'accesses';
  FULL_URL = this.API_URL.concat(this.URI);

  constructor(private http: HttpClient) {
  }

  getAllAccesses(): Observable<any[]> {
    return this.http.get<any[]>(this.FULL_URL);
  }
}
