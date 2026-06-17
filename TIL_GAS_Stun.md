# TIL - GAS Stun System

---

## 1. UE 5.3+ GE 태그 부여 방식 변경

**이전 (5.2 이하)**
```cpp
InheritableGrantedTagsContainer.Added.AddTag(StunTag);
```

**이후 (5.3+)**
```cpp
UTargetTagsGameplayEffectComponent* TagsComp = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComp"));
GEComponents.Add(TagsComp);
FInheritedTagContainer TagContainer;
TagContainer.Added.AddTag(StunTag);
TagsComp->SetAndApplyTargetTagChanges(TagContainer);
```
GE 기능이 컴포넌트 방식으로 분리됨.

---

## 2. 생성자 안에서 NewObject 사용 불가

`FindOrAddComponent`는 내부적으로 `NewObject`를 사용해서 생성자 안에서 호출 시 크래시.

생성자 안에서는 반드시 `CreateDefaultSubobject` 사용.

```cpp
// ❌
FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

// ✅
CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TagsComp"));
```

---

## 3. NotifyHit는 bSweep = true 필요

`AddActorLocalOffset`의 기본값은 `bSweep = false`라서 충돌 이벤트가 발생하지 않음.
`NotifyHit`를 쓰려면 반드시 `true` 전달.

```cpp
// ❌
AddActorLocalOffset(Direction * DeltaTime * MoveSpeed);

// ✅
AddActorLocalOffset(Direction * DeltaTime * MoveSpeed, true);
```

---

## 4. TryActivateAbilityByClass는 정확한 클래스 매칭

내부적으로 `GetClass() ==` 비교라서 부모/자식 관계를 무시함.
`GA_Stun`으로 찾으면 `BP_GA_Stun`은 찾지 못함.

**해결: 태그 기반 활성화 사용**
```cpp
// GA_Stun 생성자
AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Stun"));

// 활성화
FGameplayTagContainer Tag;
Tag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Stun"));
TargetASC->TryActivateAbilitiesByTag(Tag);
```

---

## 5. 태그 중복 스택 문제

`State.Stun` 태그를 두 곳에서 부여하면 스택이 2 이상 쌓임.
- `ActivationOwnedTags` (GA)
- `Granted Tags` (GE)

한 곳에서만 태그를 부여해야 함. `ActivationOwnedTags`를 쓰면 GE Granted Tags는 제거.

---

## 6. ActivationBlockedTags로 중복 활성화 방지

스턴 중 플랫폼에 계속 닿으면 어빌리티가 재활성화됨.
`ActivationBlockedTags`에 `State.Stun`을 추가하면 이미 스턴 상태일 때 활성화 차단.

```cpp
ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Stun"));
```

---

## 7. GAS 표준 구조 - ASC는 PlayerState 소유

| 소유자 | 설명 |
|---|---|
| Character | 단순한 경우, AI |
| PlayerState | 플레이어 캐릭터 권장 |

`InitAbilityActorInfo(PlayerState, Character)` — Owner는 PlayerState, Avatar는 Character.
호출 타이밍은 `PossessedBy` (Controller가 연결된 직후).

```cpp
void ACH3_CharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    ACH3_PlayerState* PS = GetPlayerState<ACH3_PlayerState>();
    PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
    PS->InitAbilities();
}
```

---

## 8. GiveAbility 책임은 캐릭터에게

플랫폼이 캐릭터에게 어빌리티를 부여하는 건 책임 분리 위반.
캐릭터(또는 PlayerState)가 자신의 어빌리티를 `BeginPlay` 또는 `PossessedBy`에서 직접 등록.

```cpp
void ACH3_PlayerState::InitAbilities()
{
    for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
    {
        if (!AbilityClass) continue;
        ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
    }
}
```
