import { Component, OnInit } from '@angular/core';
import {Token} from '../../../model/token';
import {TokenService} from '../../../service/token.service';
import {ModalLockAddComponent} from '../../modals/modal-lock-add/modal-lock-add.component';
import {NgbModal} from '@ng-bootstrap/ng-bootstrap';
import {ModalTokenAddComponent} from '../../modals/modal-token-add/modal-token-add.component';
import {Lock} from '../../../model/lock';

@Component({
  selector: 'app-token-list',
  templateUrl: './token-list.component.html',
  styleUrls: ['./token-list.component.css']
})
export class TokenListComponent implements OnInit {

  tokens: Token[];

  constructor(private service: TokenService, private modalService: NgbModal) { }

  ngOnInit() {
    this.service.getAllTokens().subscribe(
      data => this.tokens = data
    );
  }

  remove(token: Token) {
    this.service.removeToken(token).subscribe( () =>
      window.location.reload()
    );
  }

  openAdd() {
    const modalRef = this.modalService.open(ModalTokenAddComponent);
  }
}
