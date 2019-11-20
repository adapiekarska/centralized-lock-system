import {Component, Input, OnInit} from '@angular/core';
import {Lock} from '../../../model/lock';
import {LockService} from '../../../service/lock.service';

@Component({
  selector: 'app-modal-lock-show',
  templateUrl: './modal-lock-show.component.html',
  styleUrls: ['./modal-lock-show.component.css']
})
export class ModalLockShowComponent implements OnInit {

  @Input() id: number;

  lock: Lock;

  constructor(private service: LockService) {
  }

  ngOnInit() {
    this.service.getLock(this.id).subscribe(
      data => this.lock = data
    );
  }

}
