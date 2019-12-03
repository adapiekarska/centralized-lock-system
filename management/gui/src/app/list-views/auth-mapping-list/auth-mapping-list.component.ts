import { Component, OnInit } from '@angular/core';
import {AuthMapping} from '../../../model/authMapping';
import {AuthMappingService} from '../../../service/auth-mapping.service';
import {ModalLockAddComponent} from '../../modals/modal-lock-add/modal-lock-add.component';
import {NgbModal} from '@ng-bootstrap/ng-bootstrap';
import {ModalAuthMappingAddComponent} from '../../modals/modal-auth-mapping-add/modal-auth-mapping-add.component';

@Component({
  selector: 'app-auth-mapping-list',
  templateUrl: './auth-mapping-list.component.html',
  styleUrls: ['./auth-mapping-list.component.css']
})
export class AuthMappingListComponent implements OnInit {

  authMappings: AuthMapping[];

  constructor(private service: AuthMappingService, private modalService: NgbModal) { }

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

  openAdd() {
    const modalRef = this.modalService.open(ModalAuthMappingAddComponent);
  }
}
