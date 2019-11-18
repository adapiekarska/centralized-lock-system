import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';
import {Observable} from 'rxjs';
import {Token} from '../model/token';

@Injectable()
export class TokenService {

  API_URL = environment.API_URL;
  URI = 'tokens';
  FULL_URL = this.API_URL.concat(this.URI);

  constructor(private http: HttpClient) {
  }

  getAllTokens(): Observable<any[]> {
    return this.http.get<any[]>(this.FULL_URL);
  }

  removeToken(token: Token): Observable<any[]> {
    return this.http.delete<any[]>(this.FULL_URL.concat(`${token.id}`));
  }

  createToken(token: Token): Observable<any[]> {
    return this.http.post<any[]>(this.FULL_URL, token);
  }
}
