import { Component, OnInit } from '@angular/core';
import {AuthMapping} from '../../../model/authMapping';
import {AuthMappingService} from '../../../service/auth-mapping.service';

@Component({
  selector: 'app-auth-mapping-list',
  templateUrl: './auth-mapping-list.component.html',
  styleUrls: ['./auth-mapping-list.component.css']
})
export class AuthMappingListComponent implements OnInit {

  authMappings: AuthMapping[];

  constructor(private service: AuthMappingService) { }

  ngOnInit() {
    this.service.getAllAuthMappings().subscribe(
      data => this.authMappings = data
    );
  }

  remove(authMapping: AuthMapping) {
    this.service.removeAuthMapping(authMapping).subscribe(
      () => window.location.reload()
    );
  }

}
