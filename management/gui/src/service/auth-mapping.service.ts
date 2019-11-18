import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';
import {Observable} from 'rxjs';
import {AuthMapping} from '../model/authMapping';

@Injectable()
export class AuthMappingService {

  API_URL = environment.API_URL;
  URI = 'auth_mappings';
  FULL_URL = this.API_URL.concat(this.URI);

  constructor(private http: HttpClient) {
  }

  getAllAuthMappings(): Observable<any[]> {
    return this.http.get<any[]>(this.FULL_URL);
  }

  removeAuthMapping(authMapping: AuthMapping): Observable<any[]> {
    return this.http.delete<any[]>(this.FULL_URL.concat(`${authMapping.id}`));
  }

}
