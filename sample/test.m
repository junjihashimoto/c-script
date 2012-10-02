#!/usr/bin/env c-script

// hello.m
#import <objc/Object.h>
#import <stdio.h>

@interface Number: Object
{
@public
    int number;
    
}

- (void)printNum;

@end

@implementation Number: Object


- (void)printNum
{
    printf("%d\n", number);
}

@end

int main(void)
{
    Number *myNumber = [Number new]; // equal to [[Number alloc] init]

    myNumber->number = 6;

    [myNumber printNum];

    return 0;
}
