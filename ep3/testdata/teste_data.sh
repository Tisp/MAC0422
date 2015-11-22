#!/bin/bash

dd if=/dev/random of=1mb.data bs=1m count=1
dd if=/dev/random of=10mb.data bs=10m count=1
dd if=/dev/random of=30mb.data bs=30m count=1
