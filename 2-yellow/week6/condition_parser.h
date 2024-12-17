#pragma once

#include "node.h"

#include <memory>
#include <iostream>

std::shared_ptr<Node> ParseCondition(std::istream &is);

void TestParseCondition();
