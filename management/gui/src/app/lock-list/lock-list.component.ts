import { Component, OnInit } from '@angular/core';
import {Lock} from '../../model/lock';
import {LockService} from '../../service/lock.service';

@Component({
  selector: 'app-lock-list',
  templateUrl: './lock-list.component.html',
  styleUrls: ['./lock-list.component.css']
})
export class LockListComponent implements OnInit {

  locks: Lock[];

  constructor(private service: LockService) { }

  ngOnInit() {
    this.service.getAllLocks().subscribe(
      data => this.locks = data
    );
  }

  remove(lock: Lock) {
    this.service.removeLock(lock);
  }

}
