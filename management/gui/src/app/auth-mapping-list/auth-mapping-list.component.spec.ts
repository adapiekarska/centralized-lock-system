import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AuthMappingListComponent } from './auth-mapping-list.component';

describe('AuthMappingListComponent', () => {
  let component: AuthMappingListComponent;
  let fixture: ComponentFixture<AuthMappingListComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AuthMappingListComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AuthMappingListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
