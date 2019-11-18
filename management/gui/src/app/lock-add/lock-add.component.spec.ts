import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { LockAddComponent } from './lock-add.component';

describe('LockAddComponent', () => {
  let component: LockAddComponent;
  let fixture: ComponentFixture<LockAddComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ LockAddComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(LockAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
