import { Component, OnInit } from '@angular/core';
import {Lock} from '../../model/lock';
import {LockService} from '../../service/lock.service';
import {NgbModal} from '@ng-bootstrap/ng-bootstrap';
import {ModalLockAddComponent} from '../modals/modal-lock-add/modal-lock-add.component';
import {ModalLockShowComponent} from '../modals/modal-lock-show/modal-lock-show.component';

@Component({
  selector: 'app-lock-list',
  templateUrl: './lock-list.component.html',
  styleUrls: ['./lock-list.component.css']
})
export class LockListComponent implements OnInit {

  locks: Lock[];

  constructor(private service: LockService, private modalService: NgbModal) { }

  ngOnInit() {
    this.service.getAllLocks().subscribe(
      data => this.locks = data
    );
  }

  remove(lock: Lock) {
    this.service.removeLock(lock).subscribe(
      () => window.location.reload()
    );
  }

  openAdd() {
    const modalRef = this.modalService.open(ModalLockAddComponent);
  }

  openShow(id) {
    const modalRef = this.modalService.open(ModalLockShowComponent);
    modalRef.componentInstance.id = id;
  }
}
