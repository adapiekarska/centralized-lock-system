import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalAuthMappingAddComponent } from './modal-auth-mapping-add.component';

describe('ModalLockAddComponent', () => {
  let component: ModalAuthMappingAddComponent;
  let fixture: ComponentFixture<ModalAuthMappingAddComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalAuthMappingAddComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalAuthMappingAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
