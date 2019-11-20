import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalLockShowComponent } from './modal-lock-show.component';

describe('ModalLockShowComponent', () => {
  let component: ModalLockShowComponent;
  let fixture: ComponentFixture<ModalLockShowComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalLockShowComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalLockShowComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
