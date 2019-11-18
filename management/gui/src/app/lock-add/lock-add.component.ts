import { Component, OnInit } from '@angular/core';
import {Lock} from '../../model/lock';
import {ActivatedRoute, Router} from '@angular/router';
import {LockService} from '../../service/lock.service';

@Component({
  selector: 'app-lock-add',
  templateUrl: './lock-add.component.html',
  styleUrls: ['./lock-add.component.css']
})
export class LockAddComponent implements OnInit {

  lock: Lock;

  constructor(private service: LockService, private route: ActivatedRoute, private router: Router) { }

  ngOnInit() {
    this.lock = new Lock();
  }

  submit() {
    this.service.createLock(this.lock).subscribe(() => this.router.navigateByUrl('/locks'));
  }
}
