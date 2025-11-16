---
title: "Document Title"
type: design-doc
section: 00-Overview
status: todo
tags:
  - tag1
  - tag2
created: YYYY-MM-DD
updated: YYYY-MM-DD
priority: medium
author:
aliases:
  - Alternative Name
---

# Document Title

Brief introduction to what this document covers.

---

## Section 1

Content here...

---

## Notes

Additional notes, open questions, or design rationale.

---

## YAML Frontmatter Guide

### Required Fields
- **title**: Human-readable document title
- **type**: Document category (overview, mechanic, system, content, technical, etc.)
- **section**: Folder/section (00-Overview, 01-Core-Mechanics, etc.)
- **status**: Current status (todo, draft, in-progress, review, complete)
- **created**: Creation date (YYYY-MM-DD)
- **updated**: Last update date (YYYY-MM-DD)

### Optional Fields
- **tags**: Array of relevant tags for searching/filtering
- **priority**: Importance (low, medium, high, critical)
- **author**: Document creator/owner
- **aliases**: Alternative names for Obsidian linking
- **dependencies**: Links to related documents
- **version**: Version number if tracking iterations

### Common Types
- `index` - Main navigation/index pages
- `section-index` - Section-level index pages
- `overview` - High-level vision and concepts
- `mechanic` - Core gameplay mechanics
- `system` - Supporting systems and architecture
- `content` - Specific content catalogs and data
- `technical` - Technical specifications
- `ui-design` - Interface and UX design
- `reference` - Reference material and research

### Common Statuses
- `todo` - Not started
- `draft` - Initial draft in progress
- `in-progress` - Actively being developed
- `review` - Ready for review/feedback
- `complete` - Finalized and approved
- `deprecated` - No longer relevant

### Common Tags
- Vision/Design: `vision`, `design-pillars`, `scope`
- Mechanics: `combat`, `movement`, `abilities`, `grid`
- Systems: `ai`, `progression`, `economy`, `saves`
- Content: `missions`, `enemies`, `classes`, `weapons`
- Meta: `research`, `base-management`, `campaign`
- Technical: `architecture`, `data-design`, `performance`
- Process: `milestone`, `task`, `risk`
