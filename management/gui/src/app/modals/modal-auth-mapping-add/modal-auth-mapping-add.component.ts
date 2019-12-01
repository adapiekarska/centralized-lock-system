import { Component, OnInit } from '@angular/core';
import {Lock} from '../../../model/lock';
import {Token} from '../../../model/token';
import {TokenService} from '../../../service/token.service';
import {LockService} from '../../../service/lock.service';
import {AuthMapping} from '../../../model/authMapping';
import {AuthMappingService} from '../../../service/auth-mapping.service';

@Component({
  selector: 'app-modal-lock-add',
  templateUrl: './modal-auth-mapping-add.component.html',
  styleUrls: ['./modal-auth-mapping-add.component.css']
})
export class ModalAuthMappingAddComponent implements OnInit {

  availableTokens: Token[];

  availableLocks: Lock[];

  authMapping: AuthMapping;

  constructor(private service: AuthMappingService) { }

  ngOnInit() {
    this.service.getAllAvailableTokens().subscribe(data => this.availableTokens = data);
    this.service.getAllAvailableLocks().subscribe(data => this.availableLocks = data);
    this.authMapping = new AuthMapping();
  }

  submitForm() {
    this.service.createAuthMapping(this.authMapping).subscribe( () =>
      window.location.reload()
    );
  }

}
