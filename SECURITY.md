# Security Policy

## Supported versions

Security fixes are applied to the current default branch.

## Reporting a vulnerability

Do not disclose exploitable vulnerabilities in a public issue. Report them through GitHub's private security advisory mechanism when it is available for this repository.

Include:
- affected commit or version
- reproducible steps
- impact
- a minimal proof of concept when safe to provide

Do not include secrets, credentials, or personal data.

## Dependency and CI security

GitHub Actions use read-only repository permissions where possible. Third-party action references are pinned to immutable commit SHAs, and downloaded Android command-line tools are verified against Google's published SHA-256 checksum.
