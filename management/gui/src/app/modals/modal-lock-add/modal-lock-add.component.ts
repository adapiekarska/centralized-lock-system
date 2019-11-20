import { Component, OnInit } from '@angular/core';
import {Lock} from '../../../model/lock';
import {Token} from '../../../model/token';
import {TokenService} from '../../../service/token.service';
import {LockService} from '../../../service/lock.service';

@Component({
  selector: 'app-modal-lock-add',
  templateUrl: './modal-lock-add.component.html',
  styleUrls: ['./modal-lock-add.component.css']
})
export class ModalLockAddComponent implements OnInit {

  availableTokens: Token[];

  lock: Lock;

  constructor(private service: LockService) { }

  ngOnInit() {
    this.service.getAllAvailableTokens().subscribe(data => this.availableTokens = data);
    this.lock = new Lock();
    this.lock.tokens = [];
  }

  submitForm() {
    this.service.createLock(this.lock).subscribe( () =>
      window.location.reload()
    );
  }

}
