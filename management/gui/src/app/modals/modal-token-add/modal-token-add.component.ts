import {Component, EventEmitter, Input, OnInit, Output} from '@angular/core';
import {Lock} from '../../../model/lock';
import {TokenService} from '../../../service/token.service';
import {Token} from '../../../model/token';
import {Router} from '@angular/router';
import {environment} from '../../../environments/environment';

@Component({
  selector: 'app-modal-token-add',
  templateUrl: './modal-token-add.component.html',
  styleUrls: ['./modal-token-add.component.css']
})
export class ModalTokenAddComponent implements OnInit {

  availableLocks: Lock[];

  token: Token;

  constructor(private service: TokenService, private router: Router) { }

  ngOnInit() {
    this.service.getAllAvailableLocks().subscribe(data => this.availableLocks = data);
    this.token = new Token();
    this.token.locks = [];
  }

  submitForm() {
    this.service.createToken(this.token).subscribe( () =>
      window.location.reload()
    );
  }
}
