import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AccessesListComponent } from './accesses-list.component';

describe('AuthMappingListComponent', () => {
  let component: AccessesListComponent;
  let fixture: ComponentFixture<AccessesListComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AccessesListComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AccessesListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
