import { Component, OnInit } from '@angular/core';
import {AuthMapping} from '../../../model/authMapping';
import {AuthMappingService} from '../../../service/auth-mapping.service';
import {Access} from '../../../model/access';
import {AccessService} from '../../../service/access.service';

@Component({
  selector: 'app-auth-mapping-list',
  templateUrl: './accesses-list.component.html',
  styleUrls: ['./accesses-list.component.css']
})
export class AccessesListComponent implements OnInit {

  accesses: Access[];

  constructor(private service: AccessService) { }

  ngOnInit() {
    this.service.getAllAccesses().subscribe(
      data => this.accesses = data
    );
  }
}
