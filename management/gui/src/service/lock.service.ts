import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';
import {Observable} from 'rxjs';
import {Lock} from '../model/lock';
import {Token} from '../model/token';

@Injectable()
export class LockService {

  API_URL = environment.API_URL;
  URI = 'locks';
  FULL_URL = this.API_URL.concat(this.URI);

  constructor(private http: HttpClient) {
  }

  getLock(id: number): Observable<Lock> {
    return this.http.get<Lock>(this.FULL_URL.concat(`/${id}`));
  }

  getAllLocks(): Observable<any[]> {
    return this.http.get<any[]>(this.FULL_URL);
  }

  removeLock(lock: Lock): Observable<any[]> {
    return this.http.delete<any[]>(this.FULL_URL.concat(`/${lock.id}`));
  }

  createLock(lock: Lock): Observable<any[]> {
    return this.http.post<any[]>(this.FULL_URL, lock);
  }

  getAllAvailableTokens(): Observable<any[]> {
    return this.http.get<any[]>(this.API_URL.concat('tokens'));
  }

}
